import { ref } from "vue";
import axios, { AxiosError } from "axios";

export const useApi = (baseUrl: string) => {
  const data = ref<any>([]);
  const loading = ref(false);
  const error = ref<string | null>(null);

  const getAll = async () => {
    loading.value = true;

    try {
      const response = await axios(baseUrl);
      data.value = response.data;
    } catch (err) {
      error.value = (err as AxiosError).message;
    } finally {
      loading.value = false;
    }
  }

  const getOne = async (id: number) => {
    const url = `${baseUrl}/${id}`;
    loading.value = true;
    try {
      const response = await axios(url);
      return response.data;
    } catch (err) {
      error.value = (err as AxiosError).message;
      return null;
    } finally {
      loading.value = false;
    }
  }

  const update = async (id: number, updatedObject: any) => {
    const url = `${baseUrl}/${id}`;
    loading.value = true;
    try {
      await axios.put(url, updatedObject)
    } catch (err) {
      error.value = (err as AxiosError).message;
    } finally {
      loading.value = false;
    }
  }

  return { data, error, loading, getAll, getOne, update };
}